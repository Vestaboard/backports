#
# Small helper library to manipulate Kconfig files
#

import os, re

src_line_rel = re.compile(r'^\s*source\s+(?P<src>[^\s"]*)"?\s*$')
tri_line = re.compile(r'^(?P<spc>\s+)tristate')
bool_line = re.compile(r'^(?P<spc>\s+)bool')
cfg_line = re.compile(r'^(?P<opt>config|menuconfig)\s+(?P<sym>[^\s]*)')
sel_line = re.compile(r'^(?P<spc>\s+)select\s+(?P<sym>[^\s]*)\s*$')
backport_line = re.compile(r'^\s+#(?P<key>[ch]-file|module-name)\s*(?P<name>.*)')

class ConfigTree(object):
    """
    If you suppport kernel integration you should use bpid.kconfig_source_var
    and set that to the the variable name you have used on your Kconfig file
    for use as a directory prefix. When packaging you only need to set this on
    the Kconfig sources part of backports. For integration this variable will
    be used to prefix all Kconfig sources taken from the kernel. This variable
    is used by Kconfig to expand to bpid.target_dir_name. All kernel Kconfig
    source entries use a full path based on bpid.project_dir, when using
    integration a prefix is needed on backported Kconfig entries to
    help ensure that the base directory used for backported code is
    bpid.target_dir and not bpid.project_dir. To help with we provide a
    verification / modifier, verify_sources(), of the Kconfig entries, you
    run this before working on the ConfigTree with any other helper.

    Verification is only needed when integrating and if the top level Kconfig
    entries that have a source entry.
    """
    def __init__(self, rootfile, bpid):
        self.bpid = bpid
        self.rootfile = os.path.basename(rootfile)
        if self.bpid.kconfig_source_var:
            self.src_line = re.compile(r'^\s*source\s+"(?P<kconfig_var>' + self.bpid.kconfig_source_var_resafe + ')?/?(?P<src>[^\s"]*)"?\s*$')
            self.kconfig_var_line = re.compile(r'.*(?P<kconfig_var>' + self.bpid.kconfig_source_var_resafe + ')+/+')
        else:
            self.src_line = re.compile(r'^\s*source\s+"(?P<src>[^\s"]*)"?\s*$')
            self.kconfig_var_line = None
        self.verified = False
        self.need_verification = False
        if self.bpid.integrate:
            for l in open(os.path.join(self.bpid.target_dir, rootfile), 'r'):
                m = self.src_line.match(l)
                mrel = src_line_rel.match(l)
                if m or mrel:
                    self.need_verification = True
                    break

    def _check_relative_source(self, f, l):
    #
    # Although we can support relative kconfig source lines its a lot safer,
    # clearer to use full paths; it also makes it easier to support / parse and
    # modify kconfig entries. The kernel also uses full paths anyway but if
    # a relative path is found we should consider changing that upstream to
    # streamline usage of full path.
        m = src_line_rel.match(l)
        if m:
            raise Exception('File: %s uses relative kconfig source entries (line: \'%s\'), use full path  with quotes' %
                            (os.path.join(self.bpid.target_dir, f), l))
    def _walk(self, f, first_pass=False):
        if self.bpid.integrate:
            if not self.bpid.kconfig_source_var and self.need_verification:
                raise Exception("You enabled integration but haven't set bpid.kconfig_source_var, this seems incorrect")
            if not first_pass and not self.verified and self.need_verification:
                raise Exception("You must run verify_sources() first, we don't do that for you as you may want to ignore some source files")
        yield f
        for l in open(os.path.join(self.bpid.target_dir, f), 'r'):
            m = self.src_line.match(l)
            if m and os.path.exists(os.path.join(self.bpid.target_dir, m.group('src'))):
                for i in self._walk(m.group('src'), first_pass=first_pass):
                    yield i
            else:
                self._check_relative_source(f, l)

    def _prune_sources(self, f, ignore):
        for nf in self._walk(f):
            out = ''
            for l in open(os.path.join(self.bpid.target_dir, nf), 'r'):
                m = self.src_line.match(l)
                if not m:
                    self._check_relative_source(nf, l)
                    out += l
                    continue
                src = m.group('src')
                if src in ignore or os.path.exists(os.path.join(self.bpid.target_dir, src)):
                        out += l
                else:
                        out += '#' + l
            outf = open(os.path.join(self.bpid.target_dir, nf), 'w')
            outf.write(out)
            outf.close()

    def verify_sources(self, ignore=[]):
        if not self.need_verification:
            return
        for nf in self._walk(self.rootfile, first_pass=True):
            out = ''
            for l in open(os.path.join(self.bpid.target_dir, nf), 'r'):
                m = self.src_line.match(l)
                if not m:
                    self._check_relative_source(nf, l)
                    out += l
                    continue
                src = m.group('src')
                k = self.kconfig_var_line.match(l)
                if src in ignore or os.path.exists(os.path.join(self.bpid.target_dir, src)):
                    if k:
                        out += l
                    else:
                        out += 'source "%s/%s"\n' % (self.bpid.kconfig_source_var,  m.group('src'))
                else:
                    if k:
                        out += '# source "' + self.bpid.kconfig_source_var + '/' + src + '"\n'
                    else:
                        out += '#' + l
            outf = open(os.path.join(self.bpid.target_dir, nf), 'w')
            outf.write(out)
            outf.close()
        # Now the kconfig_var is always required from now on
        self.src_line = re.compile(r'^\s*source\s+"(?P<kconfig_var>' + self.bpid.kconfig_source_var_resafe + ')+/+(?P<src>[^\s"]*)"?\s*$')
        self.verified = True

    def prune_sources(self, ignore=[]):
        self._prune_sources(self.rootfile, ignore)

    def force_tristate_modular(self):
        for nf in self._walk(self.rootfile):
            out = ''
            for l in open(os.path.join(self.bpid.target_dir, nf), 'r'):
                m = tri_line.match(l)
                out += l
                if m:
                    out += m.group('spc') + "depends on m\n"
            outf = open(os.path.join(self.bpid.target_dir, nf), 'w')
            outf.write(out)
            outf.close()

    def symbols(self):
        syms = []
        for nf in self._walk(self.rootfile):
            for l in open(os.path.join(self.bpid.target_dir, nf), 'r'):
                m = cfg_line.match(l)
                if m:
                    syms.append(m.group('sym'))
        return syms

    def all_selects(self):
        result = []
        for nf in self._walk(self.rootfile):
            for l in open(os.path.join(self.bpid.target_dir, nf), 'r'):
                m = sel_line.match(l)
                if m:
                    result.append(m.group('sym'))
        return result

    def modify_selects(self):
        syms = self.symbols()
        for nf in self._walk(self.rootfile):
            out = ''
            for l in open(os.path.join(self.bpid.target_dir, nf), 'r'):
                m = sel_line.match(l)
                if m and not m.group('sym') in syms:
                    if 'BACKPORT_' + m.group('sym') in syms:
                        out += m.group('spc') + "select BACKPORT_" + m.group('sym') + '\n'
                    else:
                        out += m.group('spc') + "depends on " + m.group('sym') + '\n'
                else:
                    out += l
            outf = open(os.path.join(self.bpid.target_dir, nf), 'w')
            outf.write(out)
            outf.close()

    def disable_symbols(self, syms):
        for nf in self._walk(self.rootfile):
            out = ''
            for l in open(os.path.join(self.bpid.target_dir, nf), 'r'):
                m = cfg_line.match(l)
                out += l
                if m and m.group('sym') in syms:
                    out += "\tdepends on n\n"
            outf = open(os.path.join(self.bpid.target_dir, nf), 'w')
            outf.write(out)
            outf.close()

    def add_dependencies(self, deps):
        for nf in self._walk(self.rootfile):
            out = ''
            for l in open(os.path.join(self.bpid.target_dir, nf), 'r'):
                m = cfg_line.match(l)
                out += l
                if m:
                    for dep in deps.get(m.group('sym'), []):
                        out += "\tdepends on %s\n" % dep
            outf = open(os.path.join(self.bpid.target_dir, nf), 'w')
            outf.write(out)
            outf.close()

def get_backport_info(filename):
    """
    Return a dictionary of

    CONFIG_SYMBOL => (type, C-files, H-files)

    where type is 'bool' or 'tristate' and the C-files/H-files are lists
    """
    f = open(filename, 'r')
    result = {}
    conf = None
    module_name = None

    # trick to always have an empty line last
    def append_empty(f):
        for l in f:
            yield l
        yield ''

    for line in append_empty(f):
        m = cfg_line.match(line)
        if not line.strip() or m:
            if conf and conf_type and (c_files or h_files):
                result[conf] = (conf_type, module_name, c_files, h_files)
            conf = None
            conf_type = None
            module_name = None
            c_files = []
            h_files = []
            if m:
                conf = m.group('sym')
            continue
        if not conf:
            continue
        m = tri_line.match(line)
        if m:
            conf_type = 'tristate'
            continue
        m = bool_line.match(line)
        if m:
            conf_type = 'bool'
            continue
        m = backport_line.match(line)
        if m:
            if m.group('key') == 'c-file':
                c_files.append(m.group('name'))
            elif m.group('key') == 'h-file':
                h_files.append(m.group('name'))
            elif m.group('key') == 'module-name':
                module_name = m.group('name')
    return result
