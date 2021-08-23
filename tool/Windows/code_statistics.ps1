$include_hpp_count = 0
$include_ipp_count = 0
$include_cpp_count = 0
$source_hpp_count = 0
$source_ipp_count = 0
$source_cpp_count = 0
cd ../../include
$include_hpp_count = ( dir -Recurse *.h | Get-Content | Measure-Object -Line ).Lines
$include_ipp_count = ( dir -Recurse *.cpp | Get-Content | Measure-Object -Line ).Lines
$include_cpp_count = ( dir -Recurse *.ipp | Get-Content | Measure-Object -Line ).Lines
cd ../source
$source_hpp_count = ( dir -Recurse *.h | Get-Content | Measure-Object -Line ).Lines
$source_ipp_count = ( dir -Recurse *.cpp | Get-Content | Measure-Object -Line ).Lines
$source_cpp_count = ( dir -Recurse *.ipp | Get-Content | Measure-Object -Line ).Lines

$include_count = $include_hpp_count + $include_ipp_count + $include_cpp_count
$source_count = $source_hpp_count + $source_ipp_count + $source_cpp_count
$total_count = $include_count + $source_count

echo "include_count : $include_count"
echo "source_count  : $source_count"
echo "total_count   : $total_count"
