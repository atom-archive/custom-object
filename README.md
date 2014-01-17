# custom-object

Override object's methods without harmony proxy.

## Installing

```sh
npm install custom-object
```

## Building
  * Clone the repository
  * Run `npm install`
  * Run `grunt` to compile the native and CoffeeScript code
  * Run `grunt test` to run the specs

## Usage

```coffeescript
customObject = require 'custom-object'
```

### customObject.createObject(handler)

Creates a new object with template set by `handler`.

### customObject.createConstructor(constructor, handler)

Creates a function, when the returned function is being called as constructor,
the passed `constructor` function would be used, and the created object would
use `hanlder` as template.
