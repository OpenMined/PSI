import { ClientWrapper } from './client'
import { ServerWrapper } from './server'

type PSICardinalityLibrary = {
  readonly server: ServerWrapper
  readonly client: ClientWrapper
}

/**
 * @implements PSICardinality
 */
export const PSICardinalityConstructor = ({
  serverWrapper,
  clientWrapper
}: {
  readonly serverWrapper: ServerWrapper
  readonly clientWrapper: ClientWrapper
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
     * @name PSICardinality.server
     * @type {Object}
     * @returns {Server}
     * @example
     * import PSICardinality from 'psi.js'
     * const server = await PSICardinality.Server.CreateWithNewKey()
     */
    ...(serverWrapper && { server: serverWrapper }),

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
     * @name PSICardinality.client
     * @type {Object}
     * @returns {Client}
     * @example
     * import PSICardinality from 'psi.js'
     * const client = await PSICardinality.Client.Create()
     */
    ...(clientWrapper && { client: clientWrapper })
  }
}

export default PSICardinalityConstructor
