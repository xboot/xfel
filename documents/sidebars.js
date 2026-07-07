/** @type {import('@docusaurus/plugin-content-docs').SidebarsConfig} */
const sidebars = {
  guide: [
    {
      type: 'category',
      label: '入门指南',
      link: { type: 'doc', id: 'guide/intro' },
      items: [
        'guide/intro',
        'guide/build-guide',
        'guide/getting-started',
      ],
    },
  ],
  command: [
    'command/version',
    'command/hexdump',
    'command/dump',
    'command/read32',
    'command/write32',
    'command/read',
    'command/write',
    'command/exec',
    'command/reset',
    'command/sid',
    'command/jtag',
    'command/ddr',
    'command/sign',
    'command/spinor',
    'command/spinand',
    'command/extra',
  ],
  examples: [
    'examples/f1c100s-uboot',
    'examples/f1c200s-spinand',
    'examples/f133-opensbi',
  ],
  reference: [
    'reference/support-list',
    'reference/ddr-types',
  ],
};

module.exports = sidebars;
