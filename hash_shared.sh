rm -f ./shared/hash.h
echo '#ifndef EXTERNAL_DEBUG_HASH_H' > ./shared/hash.h
echo '#define EXTERNAL_DEBUG_HASH_H' >> ./shared/hash.h
echo -n '#define SHARED_HASH "' >> ./shared/hash.h
cat ./shared/* | md5sum | cut -f 1 -d ' ' | tr -d '\n'  >> ./shared/hash.h
echo '"\n#endif' >> ./shared/hash.h

