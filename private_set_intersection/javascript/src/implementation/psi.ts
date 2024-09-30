import { ClientWrapper } from './client'
import { DataStructureWrapper } from './dataStructure'
import { PackageWrapper } from './package'
import { Request, Response, ServerSetup } from './proto/psi_pb'
import { ServerWrapper } from './server'

export type PSILibrary = {
  readonly serverSetup: typeof ServerSetup
  readonly request: typeof Request
  readonly response: typeof Response
  readonly package: PackageWrapper
  readonly dataStructure: DataStructureWrapper
  readonly server?: ServerWrapper
  readonly client?: ClientWrapper
}

type PSIConstructorOptions = {
  readonly packageWrapper: PackageWrapper
  readonly dataStructureWrapper: DataStructureWrapper
  readonly serverWrapper?: ServerWrapper
  readonly clientWrapper?: ClientWrapper
}

/**
 * @implements PSI
 */
export const PSIConstructor = ({
  packageWrapper,
  dataStructureWrapper,
  serverWrapper,
  clientWrapper
}: PSIConstructorOptions): PSILibrary => {
  /**
   * @interface PSI
   */
  return {
    /**
     * @description
     * The server setup message protobuf
     *
     * @readonly
     * @name PSI.serverSetup
     * @type {ServerSetup}
     * @example
     * import PSI from '@openmined/psi.js'
     * const psi = await PSI()
     * const { serverSetup, server } = psi
     *
     * // A ServerSetup message is returned from the server
     * const setup = server.createSetupMessage(...)
     *
     * // Serialize the object to binary data (Uint8Array) for transport
     * const binary = setup.serializeBinary()
     *
     * // Deserialize binary data into a ServerSetup object
     * const setup = serverSetup.deserializeBinary(binary)
     */
    ...{ serverSetup: ServerSetup },

    /**
     * @description
     * The client request protobuf
     *
     * @readonly
     * @name PSI.request
     * @type {Request}
     * @example
     * import PSI from '@openmined/psi.js'
     * const psi = await PSI()
     * const { Request, client } = psi
     *
     * // A Request is returned from the server
     * const clientRequest = client.createRequest(...)
     *
     * // Serialize the object to binary data (Uint8Array) for transport
     * const binary = clientRequest.serializeBinary()
     *
     * // Deserialize binary data into a Request object
     * const clientRequest = request.deserializeBinary(binary)
     */
    ...{ request: Request },

    /**
     * @description
     * The server response protobuf
     *
     * @readonly
     * @name PSI.response
     * @type {Response}
     * @example
     * import PSI from '@openmined/psi.js'
     * const psi = await PSI()
     * const { Response, server } = psi
     *
     * // A Response is returned from the server
     * const serverResponse = server.processRequest(...)
     *
     * // Serialize the object to binary data (Uint8Array) for transport
     * const binary = serverResponse.serializeBinary()
     *
     * // Deserialize binary data into a Response object
     * const serverResponse = request.deserializeBinary(binary)
     */
    ...{ response: Response },

    /**
     * @description
     * Information about the package
     *
     * @readonly
     * @name PSI.package
     * @type {Object}
     * @example
     * import PSI from '@openmined/psi.js'
     * const psi = await PSI()
     * const { version } = psi.package
     */
    ...{ package: packageWrapper },

    /**
     * @description
     * Supported data structures for the server
     *
     * @readonly
     * @name PSI.dataStructure
     * @type {Object}
     * @example
     * import PSI from '@openmined/psi.js'
     * const psi = await PSI()
     * ...
     * const setup = server.createSetupMessage(
     *   fpr,
     *   numClientElements,
     *   serverInputs,
     *   psi.dataStructure.BloomFilter // GCS is the default
     * )
     */
    ...{ dataStructure: dataStructureWrapper },

    /**
     * @description
     * The server side of a Private Set Intersection protocol.
     * See the documentation in PSI.Client for a full description of the
     * protocol.
     *
     * @readonly
     * @name PSI.server
     * @type {Object}
     * @returns {Server}
     * @example
     * import PSI from '@openmined/psi.js'
     * const psi = await PSI()
     * const server = psi.server.createWithNewKey()
     */
    ...(serverWrapper && { server: serverWrapper }),

    /**
     * @description
     * Client side of a Private Set Intersection protocol. In
     * PSI, two parties (client and server) each hold a dataset, and at
     * the end of the protocol the client learns the size of the intersection of
     * both datasets, while no party learns anything beyond that.
     *
     * This variant of PSI introduces a small false-positive rate (i.e.,
     * the reported cardinality will be slightly larger than the actual cardinality.
     * The false positive rate can be tuned by the server.
     *
     * @readonly
     * @name PSI.client
     * @type {Object}
     * @returns {Client}
     * @example
     * import PSI from '@openmined/psi.js'
     * const psi = await PSI()
     * const client = psi.client.createWithNewKey()
     */
    ...(clientWrapper && { client: clientWrapper })
  }
}
