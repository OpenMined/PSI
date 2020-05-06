export type Server = {
  readonly delete: () => void
  readonly CreateSetupMessage: (
    fpr: number,
    numClientInputs: number,
    inputs: readonly string[]
  ) => string
  readonly ProcessRequest: (clientRequest: string) => string
  readonly GetPrivateKeyBytes: () => Uint8Array
}

export type Client = {
  readonly delete: () => void
  readonly CreateRequest: (clientInputs: readonly string[]) => {}
  readonly ProcessResponse: (serverSetup: string, serverResponse: string) => {}
}

export type Status = {
  readonly Message: string
}

type CppResult = {
  readonly Status?: Status
  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  readonly Value: any
}

export type CreateSetupMessageResult = CppResult & {
  readonly Value: string
}
export type CreateRequestResult = CppResult & {
  readonly Value: string
}
export type ProcessRequestResult = CppResult & {
  readonly Value: string
}
export type ProcessResponseResult = CppResult & {
  readonly Value: number
}
export type CreateClientResult = CppResult & {
  readonly Value: Client
}
export type CreateServerResult = CppResult & {
  readonly Value: Server
}

export type CppLibrary = {
  onRuntimeInitialized: () => void
  readonly delete: () => void
  readonly CreateSetupMessage: (
    fpr: number,
    numClientInputs: number,
    inputs: readonly string[]
  ) => CreateSetupMessageResult
  readonly CreateRequest: (inputs: readonly string[]) => CreateRequestResult
  readonly ProcessRequest: (clientRequest: string) => ProcessRequestResult
  readonly ProcessResponse: (
    setup: string,
    response: string
  ) => ProcessResponseResult
  readonly GetPrivateKeyBytes: () => Uint8Array
  readonly PSICardinalityClient: {
    readonly Create: () => CreateClientResult
  }
  readonly PSICardinalityServer: {
    readonly CreateWithNewKey: () => CreateServerResult
    readonly CreateFromKey: (key: Uint8Array) => CreateServerResult
  }
}
