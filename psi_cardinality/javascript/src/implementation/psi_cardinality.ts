import { ClientLibrary } from './client'
import { ServerLibrary } from './server'

type PSICardinalityLibrary = {
  readonly Server: ServerLibrary
  readonly Client: ClientLibrary
}

/**
 * @implements PSICardinality
 */
export const PSICardinalityImpl = ({
  ServerImpl,
  ClientImpl
}: {
  readonly ServerImpl: ServerLibrary
  readonly ClientImpl: ClientLibrary
}): PSICardinalityLibrary => {
  /**
   * @interface PSICardinality
   */
  return {
    /**
     * @description
     * The server side of a Private Set Intersection Cardinality protocol.
     * See the documentation in PSICardinality.Client for a full description of the
     * protocol.
     *
     * @readonly
     * @name PSICardinality.Server
     * @type {Object}
     * @returns {Server}
     * @example
     * import PSICardinality from 'psi.js'
     * const server = await PSICardinality.Server.CreateWithNewKey()
     */
    ...(ServerImpl && { Server: ServerImpl }),

    /**
     * @description
     * Client side of a Private Set Intersection-Cardinality protocol. In
     * PSI-Cardinality, two parties (client and server) each hold a dataset, and at
     * the end of the protocol the client learns the size of the intersection of
     * both datasets, while no party learns anything beyond that.
     *
     * This variant of PSI-Cardinality introduces a small false-positive rate (i.e.,
     * the reported cardinality will be slightly larger than the actual cardinality.
     * The false positive rate can be tuned by the server.
     *
     * @readonly
     * @name PSICardinality.Client
     * @type {Object}
     * @returns {Client}
     * @example
     * import PSICardinality from 'psi.js'
     * const client = await PSICardinality.Client.Create()
     */
    ...(ClientImpl && { Client: ClientImpl })
  }
}

export default PSICardinalityImpl
