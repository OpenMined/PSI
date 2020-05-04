import cpp_main from '.'
import Library from './src/psiServer'
;(async () => {
  // Wait for the initializer to complete loading
  // Upon success, this will immediatly run our C++ main() function
  const { library } = await cpp_main(
    `../../bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_server_${process.env.RUN_DEMO}.js`
  ) // RUN_DEMO = js|wasm

  const PSICardinality = Library(library)

  const server = PSICardinality.Server.CreateWithNewKey()

  const bytes = server.GetPrivateKeyBytes()
  console.log(`Key Bytes: '${bytes}'`)

  const numClientElements = 10
  const numServerElements = 100

  const serverInputs = Array.from(
    { length: numServerElements },
    (_, i) => `Element ${i}`
  )

  console.time('Server Setup Message')
  const setup = server.CreateSetupMessage(
    0.001,
    numClientElements,
    serverInputs
  )
  console.timeEnd('Server Setup Message')
  console.log(`setup: '${setup}'`)

  const clientRequest =
    '["AiHdmxkmF/iOM0fFhny9917QYGcb9jq0GM9mP4L74ecM","AiK1rXzYeZsUYwNR9lYXBBmYlcTFMqLCcHjcrsaGmjYv","ArWL4Au/vSMRwzMSoQp/wx79LMbE9QVXUuUs0fQC7+jV","As86QtTWj3zXwEaNUszxND+XCggown/JmLjOEAub6gNZ","AwQIw0Q2APjfh4pZuezodFarGUObZuRsnSHRt6UUyEIW","AxUyZOrfnfSsfoKn6WU+MogxV86O84wGfY4YeIDuODV4","A1pd7KrYwkpOQACwUW4dzIHqqJLGqy3KEkpRGZoyjOyN","A5WV5wuo7dtwJ5VCU1gZZ6UIVEZ4nPEGSSzML/F2jsaz","A8kWr10ssqPj5HuPaM0M+sT3yK2NcLivaeb6OL//0uYK","A9kFLnXzVbmd0eV37TtL1d5vNl5IkKdaM1KZuhfnmD8Z"]'

  console.time('Server Process Request')
  const response = server.ProcessRequest(clientRequest)
  console.timeEnd('Server Process Request')
  console.log(response)
})()
