import { Server as PSIServerImpl } from './implementation/server'

/**
 * @implements PSICardinality
 */
const PSICardinalityImpl = library => {
  const Server = PSIServerImpl(library)

  /**
   * @interface PSICardinality
   */
  return {
    /**
     * @description
     * The server side of a Private Set Intersection Cardinality protocol.
     * See the documentation in PSICardinalityClient for a full description of the
     * protocol.
     *
     * @readonly
     * @name PSICardinality.Server
     * @type {Object}
     * @returns {Server}
     * @example
     * import PSICardinality from 'psi.js/server'
     * const server = PSICardinality.Server.CreateWithNew()
     */
    Server
  }
}

export default PSICardinalityImpl
