import { terser } from 'rollup-plugin-terser'

const targets = ['client', 'server', 'combined']
const variants = ['wasm', 'js']
const formats = ['umd', 'es']

const outputs = targets.reduce(
  (acc, t) => [
    ...acc,
    ...variants.reduce(
      (acc, v) => [
        ...acc,
        {
          input: `psi_cardinality/javascript/src/index_${t}_${v}.js`,
          output: formats.reduce(
            (acc, f) => [
              ...acc,
              {
                file: `psi_cardinality/javascript/dist/${t}/${v}/${f}/index.js`,
                sourcemap: true,
                format: `${f}`,
                name: 'PSICardinality',
                plugins: [terser()]
              }
            ],
            []
          )
        }
      ],
      []
    )
  ],
  []
)

export default outputs
