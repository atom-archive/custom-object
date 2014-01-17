{
  'targets': [
    {
      'target_name': 'custom-object',
      'include_dirs': [ '<!(node -e "require(\'nan\')")' ],
      'sources': [
        'src/custom-object.cc',
      ],
    }
  ]
}
