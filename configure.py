#!/usr/bin/env python
# ---------------------------------------------------------------------------------------
# configure.py: Athena++ configuration script in python. Original version by CJW.
#
# When configure.py is run, it uses the command line options and default settings to
# create custom versions of the files Makefile and src/defs.hpp from the template files
# Makefile.in and src/defs.hpp.in respectively.
#
# The following options are implememted:
#   -h, --help        show this help message and exit
#   --model=name      use src/user_models/name.cpp as the model definition file
#   --dust=xxx        use xxx as the dust module
#   -noomp            disable OpenMP parallelization
# ----------------------------------------------------------------------------------------

# Modules
import argparse
import glob
import re

# Set template and output filenames
makefile_input = 'Makefile.in'
makefile_output = 'Makefile'
defsfile_input = 'src/defs.hpp.in'
defsfile_output = 'src/defs.hpp'

# --- Step 1. Prepare parser, add each of the arguments ------------------
parser = argparse.ArgumentParser()

# --model=[name] argument
user_model_directory = 'src/user_models/'
# set pgen_choices to list of .cpp files in src/pgen/
user_model_choices = glob.glob(user_model_directory + '*.cpp')
# remove 'src/user_models/' prefix and '.cpp' extension from each filename
user_model_choices = [choice[len(user_model_directory):-4] for choice in user_model_choices]
parser.add_argument('--model',
                    default='IRS',
                    choices=user_model_choices,
                    help='select user-defined model')

# --dust=[name] argument
parser.add_argument('--dust',
                    default='electrostatic',
                    choices=['electrostatic', 'spherical_p0'],
                    help='select dust module')

# -noomp argument
parser.add_argument('-noomp',
                    action='store_true',
		    default=False,
		    help='disable parallelization with OpenMP')

# Parse command-line inputs
args = vars(parser.parse_args())

# --- Step 3. Set definitions and Makefile options based on above argument

# Prepare dictionaries of substitutions to be made
definitions = {}
makefile_options = {}
makefile_options['LOADER_FLAGS'] = ''

# only support g++ so far
definitions['COMPILER_CHOICE'] = 'g++'
definitions['COMPILER_COMMAND'] = makefile_options['COMPILER_COMMAND'] = 'g++'
makefile_options['PREPROCESSOR_FLAGS'] = ''
makefile_options['COMPILER_FLAGS'] = '-O3 -std=c++11'
makefile_options['LINKER_FLAGS'] = ''
makefile_options['LIBRARY_FLAGS'] = ''

# --model=[name] argument
definitions['PROBLEM'] = makefile_options['PROBLEM_FILE'] = args['model']

# --dust=[name] argument
definitions['DUST_MODULE'] = makefile_options['DUST_MODULE_PATH'] = args['dust']

# -noomp argument
if not args['noomp']:
    definitions['OPENMP_OPTION'] = 'OPENMP_PARALLEL'
    makefile_options['COMPILER_FLAGS'] += ' -fopenmp'
else:
    definitions['OPENMP_OPTION'] = 'NOT_OPENMP_PARALLEL'

# Assemble all flags of any sort given to compiler
definitions['COMPILER_FLAGS'] = ' '.join(
    [makefile_options[opt+'_FLAGS'] for opt in
     ['PREPROCESSOR', 'COMPILER', 'LINKER', 'LIBRARY']])

# --- Step 4. Create new files, finish up --------------------------------

# Terminate all filenames with .cpp extension
makefile_options['PROBLEM_FILE'] += '.cpp'

# Read templates
#with open(defsfile_input, 'r') as current_file:
#    defsfile_template = current_file.read()
with open(makefile_input, 'r') as current_file:
    makefile_template = current_file.read()

# Make substitutions
#for key, val in definitions.items():
#    defsfile_template = re.sub(r'@{0}@'.format(key), val, defsfile_template)
for key, val in makefile_options.items():
    makefile_template = re.sub(r'@{0}@'.format(key), val, makefile_template)

# Write output files
#with open(defsfile_output, 'w') as current_file:
#    current_file.write(defsfile_template)
with open(makefile_output, 'w') as current_file:
    current_file.write(makefile_template)

# Finish with diagnostic output
print('Your SSRT distribution has now been configured with the following options:')
print('  User-defined model       ' + args['model'])
print('  Dust module:             ' + args['dust'])
print('  OpenMP parallelism:      ' + ('ON' if not args['noomp'] else 'OFF'))
print('  Compilation command:     ' + 'g++ '
      + makefile_options['PREPROCESSOR_FLAGS'] + ' ' + makefile_options['COMPILER_FLAGS'])
