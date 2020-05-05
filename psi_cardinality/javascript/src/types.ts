export type Status = {
  readonly Message: string
}

type CppReturnObject = {
  readonly Status?: Status
  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  readonly Value: any
}

export type SetupMessage = CppReturnObject & {
  readonly Value: string
}
export type Request = CppReturnObject & {
  readonly Value: string
}
export type ProcessedRequest = CppReturnObject & {
  readonly Value: string
}
export type Response = CppReturnObject & {
  readonly Value: number
}

export type CppLibraryInstance = {
  onRuntimeInitialized: () => void
  readonly delete: () => void
  readonly CreateSetupMessage: (
    fpr: number,
    numClientInputs: number,
    inputs: readonly string[]
  ) => SetupMessage
  readonly CreateRequest: (inputs: readonly string[]) => Request
  readonly ProcessRequest: (clientRequest: string) => ProcessedRequest
  readonly ProcessResponse: (setup: string, response: string) => Response
  readonly GetPrivateKeyBytes: () => Uint8Array
  readonly PSICardinalityClient: {
    readonly Create: () => CppReturnObject
  }
  readonly PSICardinalityServer: {
    readonly CreateWithNewKey: () => CppReturnObject
    readonly CreateFromKey: (key: Uint8Array) => CppReturnObject
  }
}
