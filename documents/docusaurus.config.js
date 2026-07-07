module.exports = {
  title: 'XFEL 文档',
  url: 'https://xfel.xboot.org',
  baseUrl: '/',
  onBrokenLinks: 'warn',
  markdown: {
    hooks: {
      onBrokenMarkdownLinks: 'warn',
    },
  },
  presets: [
    [
      'classic',
      /** @type {import('@docusaurus/preset-classic').Options} */
      ({
        docs: {
          path: 'docs',
          routeBasePath: '/',
          sidebarPath: require.resolve('./sidebars.js'),
        },
        blog: false,
        theme: {
          customCss: require.resolve('./src/css/custom.css'),
        },
      }),
    ],
  ],
  plugins: [
    [
      require.resolve('@easyops-cn/docusaurus-search-local'),
      {
        hashed: true,
        language: ['zh', 'en'],
        indexBlog: false,
        indexDocs: true,
        docsRouteBasePath: '/',
        explicitSearchResultPath: true,
        highlightSearchTermsOnTargetPage: true,
      },
    ],
  ],
  themeConfig:
    /** @type {import('@docusaurus/preset-classic').ThemeConfig} */
    ({
      metadata: [{ name: 'keywords', content: 'xfel, allwinner, fel, sunxi, soc, usb' }],
      colorMode: {
        defaultMode: 'light',
        disableSwitch: false,
        respectPrefersColorScheme: true,
      },
      navbar: {
        title: 'XFEL',
        items: [
          {
            type: 'docSidebar',
            sidebarId: 'guide',
            position: 'left',
            label: '入门指南',
          },
          {
            type: 'docSidebar',
            sidebarId: 'command',
            position: 'left',
            label: '命令参考',
          },
          {
            type: 'docSidebar',
            sidebarId: 'examples',
            position: 'left',
            label: '使用示例',
          },
          {
            type: 'docSidebar',
            sidebarId: 'reference',
            position: 'left',
            label: '参考列表',
          },
          {
            type: 'localeDropdown',
            position: 'right',
          },
        ],
      },
      footer: {
        style: 'dark',
        copyright: `Copyright © ${new Date().getFullYear()} XFEL. Built with Docusaurus.`,
      },
      prism: {
        theme: require('prism-react-renderer').themes.github,
        darkTheme: require('prism-react-renderer').themes.dracula,
        additionalLanguages: ['bash', 'c', 'cpp', 'json', 'makefile'],
      },
      tableOfContents: {
        minHeadingLevel: 2,
        maxHeadingLevel: 4,
      },
    }),
  i18n: {
    defaultLocale: 'zh',
    locales: ['zh', 'en'],
  },
};
