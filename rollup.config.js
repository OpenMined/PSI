import { terser } from 'rollup-plugin-terser'
import alias from '@rollup/plugin-alias'

const targets = ['client', 'server', 'combined']
const variants = ['wasm', 'js']
const formats = ['umd', 'es']

const outputs = targets.reduce(
  (acc, target) => [
    ...acc,
    ...variants.reduce(
      (acc, variant) => [
        ...acc,
        {
          input: `psi_cardinality/javascript/tsc-out/index_${target}_${variant}.js`,
          output: formats.reduce(
            (acc, format) => [
              ...acc,
              {
                file: `psi_cardinality/javascript/dist/${target}/${variant}/${format}/index.js`,
                sourcemap: true,
                format,
                name: 'PSICardinality',
                plugins: [terser()]
              }
            ],
            []
          ),
          plugins: [
            alias({
              entries: [
                {
                  find: /^psi_cardinality(.*)$/,
                  replacement:
                    './psi_cardinality/javascript/bin/psi_cardinality$1.js'
                }
              ]
            })
          ]
        }
      ],
      []
    )
  ],
  []
)

export default outputs
