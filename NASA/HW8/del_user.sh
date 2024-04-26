#!/bin/bash
NAME=$1

ldapdelete -D cn=admin,dc=nasa,dc=csie,dc=ntu -w nasa2024 uid=${NAME},ou=people,dc=nasa,dc=csie,dc=ntu
