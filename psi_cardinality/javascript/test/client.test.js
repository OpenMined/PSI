import cpp_main from '../'
import Library from '../src/psiClient'

let Client = null
beforeAll(async () => {
  // Wait for the initializer to complete loading
  // Upon success, this will immediatly run our C++ main() function
  const { library } = await cpp_main(
    `../../bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_client_${process.env.RUN_DEMO}.js`
  ) // RUN_DEMO = js|wasm

  const PSICardinality = Library(library)

  Client = PSICardinality.Client.Create()
})

describe('PSI Client', () => {
  test('It should create a request', () => {
    const numClientElements = 100
    const clientInputs = Array.from(
      { length: numClientElements },
      (_, i) => `Element ${i}`
    )
    const request = Client.CreateRequest(clientInputs)

    expect(typeof request).toBe('string')
  })
  test('It should create a request', () => {
    const numClientElements = 100
    const clientInputs = Array.from(
      { length: numClientElements },
      (_, i) => `Element ${i}`
    )
    const request = Client.CreateRequest(clientInputs)

    expect(typeof request).toBe('string')
  })
})
