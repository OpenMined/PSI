declare module 'bazel-psi-cardinality' {
  type Status = {
    readonly Message: string
  }

  type Result = {
    readonly Status?: Status
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    readonly Value: any
  }

  type CreateSetupMessageResult = Result & {
    readonly Value: string
  }
  type CreateRequestResult = Result & {
    readonly Value: string
  }
  type ProcessRequestResult = Result & {
    readonly Value: string
  }
  type ProcessResponseResult = Result & {
    readonly Value: number
  }
  type CreateClientResult = Result & {
    readonly Value: Client
  }
  type CreateServerResult = Result & {
    readonly Value: Server
  }

  export type Server = {
    readonly delete: () => void
    readonly CreateSetupMessage: (
      fpr: number,
      numClientInputs: number,
      inputs: readonly string[]
    ) => CreateSetupMessageResult
    readonly ProcessRequest: (clientRequest: string) => ProcessRequestResult
    readonly GetPrivateKeyBytes: () => Uint8Array
  }

  export type Client = {
    readonly delete: () => void
    readonly CreateRequest: (
      clientInputs: readonly string[]
    ) => CreateRequestResult
    readonly ProcessResponse: (
      serverSetup: string,
      serverResponse: string
    ) => ProcessResponseResult
  }

  export type Library = {
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
}
