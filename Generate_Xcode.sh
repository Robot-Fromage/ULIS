set -e
cd "`dirname "$0"`"

if [ -f ULIS/Tools/Scripts/Mac/Generate_Xcode.sh ]; then
    sh ULIS/Tools/Scripts/Mac/Generate_Xcode.sh
fi