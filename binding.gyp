{
  'targets': [
    {
      'target_name': 'custom-object',
      'include_dirs': [ '<!(node -e "require(\'nan\')")' ],
      'sources': [
        'src/custom_object.cc',
        'src/object_helper.h',
      ],
    }
  ]
}
