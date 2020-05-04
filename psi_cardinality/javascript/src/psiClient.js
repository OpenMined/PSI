import * as Components from '../components'
import { applyDependencies, addLibraryToComponents } from './util'

export const PSI = library => {
  // Inject all components with the seal wasm library
  const components = addLibraryToComponents(library)(Components)

  // Next, create our singleton and constructor modules in order of dependence
  const Exception = applyDependencies()(components.Exception)
  const Client = applyDependencies({ Exception })(components.Client)

  /**
   * @implements PSICardinality
   */

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
     * import PSIClient from 'psi.js/client'
     * const client = PSIClient.Create()
     */
    Client,

    /**
     * @description
     * The Exception singleton. Users should have little use
     * for this static instance as it is used internally.
     *
     * @readonly
     * @name PSICardinality.Exception
     * @type {Exception}
     * @returns {Error} A JavaScript Error instance
     * @example
     * import PSIClient from 'psi.js/client'
     * const client = PSIClient.Create()
     * ...
     * try {
     *   client.<method that will throw>
     * } catch (e) {
     *   throw PSI.Exception.safe(e)
     * }
     */
    Exception
  }
}
