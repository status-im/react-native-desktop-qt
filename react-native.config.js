module.exports = {
    commands: [
      require('./local-cli/runDesktop/runDesktop'),
    ],
    platforms: {
      "desktop-qt": {
          linkConfig: () => null,
          projectConfig: (projectRoot, projectParams) => null,
          dependencyConfig: (projectRoot, dependencyParams) => null,
      },
    },
};
