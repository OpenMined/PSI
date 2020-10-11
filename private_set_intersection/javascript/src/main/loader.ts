import * as psi from 'psi_'

export type Loader = {
  readonly library: psi.Library
}

/**
 * Export a default function which instantiates the library
 * @param {Object} bin Emscripten library to initialize
 */
export const createLoader = async (
  bin: () => Promise<psi.Library>
): Promise<Loader> => ({
  library: await bin()
})
