import { Loader } from './loader'
import { PSICardinalityImpl } from './implementation/psi_cardinality'
import { ServerImpl } from './implementation/server'
import { ClientImpl } from './implementation/client'

const serverPath = `../../../bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_server_${process.env.RUN_DEMO}.js`
const clientPath = `../../../bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_client_${process.env.RUN_DEMO}.js`

/**
 * Main export for the library
 */
export default PSICardinalityImpl({
  ServerImpl: ServerImpl({ Loader: Loader(serverPath) }),
  ClientImpl: ClientImpl({ Loader: Loader(clientPath) })
})
