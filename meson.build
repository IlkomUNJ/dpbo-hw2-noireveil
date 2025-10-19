project('online_store', 'cpp',
  version : '1.0',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

# === TAMBAHKAN INI ===
# Cari dependency Boost (Serialization dan System)
boost_dep = dependency('boost', modules : ['system', 'serialization'], required : true)
# =====================

# Source files
src_files = [
  'src/Bank.cpp',
  'src/BankAccount.cpp',
  'src/BankTransaction.cpp',
  'src/Buyer.cpp',
  'src/Item.cpp',
  'src/Order.cpp',
  'src/Seller.cpp',
  'src/Store.cpp',
  'src/Utils.cpp',
  'main.cpp'  # main.cpp berada di root directory
]

# Include directories
inc = include_directories('include')

# Executable
exe = executable('online_store',
  src_files,
  include_directories : inc,
  dependencies : [boost_dep],  # <--- TAMBAHKAN INI
  install : true)

# Cross-platform run target (Sudah benar)
if host_machine.system() == 'windows'
  # Windows: gunakan cmd
  run_target('run',
    command : ['cmd', '/c', 'cd', meson.project_source_root(), '&&', exe.full_path()]
  )
else
  # Linux/Unix: gunakan sh
  run_target('run',
    command : ['sh', '-c', 'cd ' + meson.project_source_root() + ' && ' + exe.full_path()]
  )
endif