customObject = require '../lib/custom-object'

describe 'custom-object', ->
  describe '.createObject(options)', ->
    it 'sets the accessor getter', ->
      date = new Date
      object = customObject.createObject
        accessor:
          name: 'prop'
          getter: (property) ->
            expect(property).toBe('prop')
            date
      expect(object.prop).toBe(date)
      object.prop = 2
      expect(object.prop).toBe(date)

    it 'sets the accessor setter', ->
      object = customObject.createObject
        accessor:
          name: 'prop'
          getter: (property) ->
            @test
          setter: (property, value) ->
            @test = value + 1
      object.prop = 1
      expect(object.prop).toBe(2)

    it 'sets the indexed property getter', ->
      date = new Date
      object = customObject.createObject
        index:
          getter: (index) ->
            expect(index).toBe(0)
            date
      object[0] = 1
      expect(object[0]).toBe(date)
