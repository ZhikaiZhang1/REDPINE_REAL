mkdir new_certs
cd new_certs/
mkdir sslcert
chmod 0700 sslcert
cd sslcert
mkdir certs private
echo '100001' >serial
touch certindex.txt

#CA root
openssl req -new -x509 -extensions v3_ca -keyout private/cakey.pem -out cacert.pem -days 365 -config ../../openssl.cnf 

#Certificates Request
openssl req -new -nodes -out redpine-req.pem -keyout private/redpine-key.pem -days 365 -config ../../openssl.cnf 

#Signing the certificates with ca root certificate generated in section CA root
openssl ca -out redpine-cert.pem -days 365 -config ../../openssl.cnf -infiles redpine-req.pem

#Finally concatenate the redpine-key.pem, redpine-cert.pem, cacert.pem in the order
cat private/redpine-key.pem redpine-cert.pem cacert.pem > cert.pem
