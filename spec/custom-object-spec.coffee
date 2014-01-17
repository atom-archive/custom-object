customObject = require '../lib/custom-object'

describe 'custom-object', ->
  describe '.createObject(handler)', ->
    it 'sets the accessor getter', ->
      date = new Date
      object = customObject.createObject
        accessor: [
          name: 'prop'
          getter: (property) ->
            expect(property).toBe('prop')
            date
        ]
      expect(object.prop).toBe(date)
      object.prop = 2
      expect(object.prop).toBe(date)

    it 'sets the accessor setter', ->
      object = customObject.createObject
        accessor: [
          name: 'prop'
          getter: (property) ->
            @test
          setter: (property, value) ->
            @test = value + 1
        ]
      object.prop = 1
      expect(object.prop).toBe(2)

    it 'can set multiple accessors', ->
      object = customObject.createObject
        accessor: [
          {
            name: 'prop1'
            getter: (property) -> 1
          }, {
            name: 'prop2'
            getter: (property) -> 2
          }
        ]
      expect(object.prop1).toBe(1)
      expect(object.prop2).toBe(2)

    it 'sets the indexed property getter', ->
      date = new Date
      object = customObject.createObject
        index:
          getter: (index) ->
            expect(index).toBe(0)
            date
      object[0] = 1
      expect(object[0]).toBe(date)

    it 'sets the indexed property enumerator', ->
      object = customObject.createObject
        index:
          data: new Array
          getter: (index, data) ->
            data[index]
          setter: (index, value, data) ->
            data[index] = value + 1
          deleter: (index, data) ->
            delete data[index]
          enumerator: (data) ->
            (i for k,i in data)
      object[0] = 1
      object[1] = 2
      object[2] = 3
      expect(object).toEqual([2, 3, 4])

    it 'generates an iteratable array when length is set', ->
      data = new Array
      object = customObject.createObject
        index:
          getter: (index) ->
            data[index]
          setter: (index, value) ->
            data[index] = value
          deleter: (index) ->
            delete data[index]
        accessor: [
          name: 'length'
          getter: ->
            data.length
          setter: (value) ->
            data.length = value
        ]
      object[0] = 1
      object[1] = 2
      object[2] = 3
      copy = []
      copy.push value for value in object
      expect(object).toEqual(copy)

  describe '.createConstructor(constructor, handler)', ->
    it 'returns a function with the same name of constructor', ->
      class TestConstructor
      constructor = customObject.createConstructor TestConstructor, {}
      expect(constructor.name).toBe(TestConstructor.name)

    it 'calls the constructor when construting an object', ->
      date = new Date
      newConstructor = null
      constructor = (arg) ->
        expect(arg).toBe(date)
        expect(@constructor).toBe(newConstructor)
      newConstructor = customObject.createConstructor constructor, {}
      object = new newConstructor(date)
      expect(object.constructor).toBe(newConstructor)

    it 'sets the accessor getter for object constructed', ->
      date = new Date
      constructor = ->
      newConstructor = customObject.createConstructor constructor,
        accessor: [
          name: 'prop'
          getter: (property) ->
            expect(property).toBe('prop')
            date
        ]
      object = new newConstructor
      expect(object.prop).toBe(date)
      object.prop = 2
      expect(object.prop).toBe(date)
