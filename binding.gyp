{
  'targets': [
    {
      'target_name': 'custom-object',
      'include_dirs': [ '<!(node -e "require(\'nan\')")' ],
      'sources': [
        'src/custom_object.cc',
        'src/arraysize.h',
        'src/object_helper.h',
      ],
    }
  ]
}
