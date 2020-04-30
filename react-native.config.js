module.exports = {
  platforms: {
    commands: [
      require('./local-cli/runDesktop/runDesktop'),
    ],
    desktop-qt: {
        linkConfig: () => null,
        projectConfig: (projectRoot, projectParams) => null,
        dependencyConfig: (projectRoot, dependencyParams) => null,
      },
  },
};
