#!/bin/bash

NAME=$1
USER_PASSWD=$2
ENCRYPTED_PASSWD=$(slappasswd -s ${USER_PASSWD})

MX_UID=$(ldapsearch -D cn=admin,dc=nasa,dc=csie,dc=ntu -w nasa2024 -b ou=people,dc=nasa,dc=csie,dc=ntu uidnumber | grep -e '^uid' | cut -d':' -f2 | sort -n | tail -1)
if [ -z ${MX_UID} ]
then
  USER_UID=1
else
  ((USER_UID=${MX_UID}+1))
fi

LDIF=$(cat << EOF
dn: uid=${NAME},ou=people,dc=nasa,dc=csie,dc=ntu
objectClass: top
objectClass: account
objectClass: posixAccount
objectClass: shadowAccount
cn: ${NAME}
uid: ${NAME}
uidNumber: ${USER_UID}
gidNumber: 200
homeDirectory: /home/${NAME}
loginShell: /bin/bash
userPassword: ${ENCRYPTED_PASSWD}
EOF
)
echo "$LDIF" | ldapadd -D cn=admin,dc=nasa,dc=csie,dc=ntu -w nasa2024
