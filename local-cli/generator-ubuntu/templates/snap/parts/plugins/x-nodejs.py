
import os
import platform

import snapcraft
from snapcraft import sources


_NODEJS_BASE = 'node-v{version}-linux-{arch}'
_NODEJS_VERSION = '4.2.6'
_NODEJS_TMPL = 'https://nodejs.org/dist/v{version}/{base}.tar.gz'
_NODEJS_ARCHES = {
    'i686': 'x86',
    'x86_64': 'x64',
    'armv7l': 'armv7l',
}


class NodeJsPlugin(snapcraft.BasePlugin):
  @classmethod
  def schema(cls):
    schema = super().schema()

    schema['properties']['node_version'] = {
      'type': 'string',
      'default': _NODEJS_VERSION
    }

    if 'required' in schema:
      del schema['required']

    return schema

  def __init__(self, name, options, project):
    super().__init__(name, options, project)
    self._nodejs_dir = os.path.join(self.partdir, 'node')
    self._nodejs_tar = sources.Tar(get_nodejs_release(
        self.options.node_version), self._nodejs_dir)

  def pull(self):
    super().pull()
    os.makedirs(self._nodejs_dir, exist_ok=True)
    self._nodejs_tar.download()

  def clean_pull(self):
    super().clean_pull()
    if os.path.exists(self._nodejs_dir):
        shutil.rmtree(self._nodejs_dir)

  def build(self):
    super().build()
    self._nodejs_tar.provision(
        self.installdir, clean_target=False, keep_tarball=True)

  def snap_fileset(self):
    return (['-bin/npm', '-CHANGELOG.md', '-LICENSE', '-README.md', '-lib', '-include', '-share'])


def get_nodejs_release(node_version):
  return _NODEJS_TMPL.format(version=node_version,
                             base=_get_nodejs_base(node_version))

def _get_nodejs_base(node_version):
  machine = platform.machine()
  if machine not in _NODEJS_ARCHES:
      raise EnvironmentError('architecture not supported ({})'.format(
          machine))
  return _NODEJS_BASE.format(version=node_version,
                             arch=_NODEJS_ARCHES[machine])

