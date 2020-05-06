import serverWasmLibrary from '../bin/psi_cardinality_server_wasm.js'
import serverJsLibrary from '../bin/psi_cardinality_server_js.js'
import clientWasmLibrary from '../bin/psi_cardinality_client_wasm.js'
import clientJsLibrary from '../bin/psi_cardinality_client_js.js'

import { Loader } from './loader'
import { PSICardinalityImpl } from './implementation/psi_cardinality'
import { ServerImpl } from './implementation/server'
import { ClientImpl } from './implementation/client'

/**
 * Detect which source to run, defaults to wasm
 */
const runWasm = process.env.RUN_DEMO
  ? Boolean(process.env.RUN_DEMO === 'wasm')
  : true

/**
 * Main export for the library
 */
export default PSICardinalityImpl({
  ServerImpl: ServerImpl({
    Loader: Loader(runWasm ? serverWasmLibrary : serverJsLibrary)
  }),
  ClientImpl: ClientImpl({
    Loader: Loader(runWasm ? clientWasmLibrary : clientJsLibrary)
  })
})
