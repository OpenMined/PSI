module.exports = {
  env: {
    browser: true,
    es6: true,
    node: true
  },
  extends: [
    'eslint:recommended',
    'plugin:@typescript-eslint/recommended',
    'prettier/@typescript-eslint',
    'plugin:prettier/recommended',
    'plugin:jest/recommended'
  ],
  parserOptions: {
    ecmaVersion: 2018,
    sourceType: 'module'
  },
  plugins: ['@typescript-eslint', 'jsdoc', 'jest', 'prettier'],
  overrides: [
    {
      files: ['**/*.js'],
      rules: {
        '@typescript-eslint/explicit-function-return-type': 'off'
      }
    },
    {
      files: ['**/__tests__/**/*.ts'],
      rules: {
        '@typescript-eslint/no-non-null-assertion': 'off'
      }
    }
  ]
}
