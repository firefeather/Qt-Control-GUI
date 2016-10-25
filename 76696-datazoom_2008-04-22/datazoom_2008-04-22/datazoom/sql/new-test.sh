#!/bin/bash
DB=test
PORT=5432
HOST=localhost
USER=admin

dropdb -h $HOST -p $PORT -U $USER $DB
createdb -h $HOST -p $PORT -U $USER -E unicode $DB
psql -h $HOST -p $PORT $DB -U $USER < ./test.sql
