import { Client as PSIClientImpl } from './implementation/client'

/**
 * @implements PSICardinality
 */
const PSICardinalityImpl = library => {
  const Client = PSIClientImpl(library)

  /**
   * @interface PSICardinality
   */
  return {
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
     * import PSICardinality from 'psi.js/client'
     * const client = PSICardinality.Client.Create()
     */
    Client
  }
}

export default PSICardinalityImpl
