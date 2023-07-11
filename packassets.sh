i=0
file=$1
shift
rm -f "$file"
touch "$file"
echo '    .global assetsdata' >> "$file"
echo '    .global assetlengths' >> "$file"
echo '    .global assetnames' >> "$file"
echo '    .global assetcount' >> "$file"
echo '    .section .rodata' >> "$file"
for asset; do
  echo 'asset'"$i"':' >> "$file"
  echo '    .incbin "'"$asset"'"' >> "$file"
  echo 'assetend'"$i"':' >> "$file"
  echo 'assetlength'"$i"':' >> "$file"
  echo '    .int assetend'"$i"' - asset'"$i"'' >> "$file"
  echo 'assetname'"$i"':' >> "$file"
  echo '    .asciz "'"$asset"'"' >> "$file"
  ((i++))
done
echo 'assetsdata:' >> "$file"
for (( j=0; j<i; j++ )); do
  echo '    .quad asset'"$j" >> "$file"
done
echo 'assetlengths:' >> "$file"
for (( j=0; j<i; j++ )); do
  echo '    .quad assetlength'"$j" >> "$file"
done
echo 'assetnames:' >> "$file"
for (( j=0; j<i; j++ )); do
  echo '    .quad assetname'"$j" >> "$file"
done
echo 'assetcount:' >> "$file"
echo '    .int '"$i" >> "$file"
echo '    .section .note.GNU-stack' >> "$file"