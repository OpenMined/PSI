import * as psiCardinality from 'psi_cardinality'

type NestedLibrary = { readonly library: psiCardinality.Library }
export type Loader = () => Promise<NestedLibrary>

/**
 * Export a default function which instantiates the library
 * @param {Object} bin Emscripten library to initialize
 */
export const createLoader = (
  bin: () => Promise<psiCardinality.Library>
): Loader => async (): Promise<NestedLibrary> => {
  const library = await bin()
  return {
    library
  }
}
