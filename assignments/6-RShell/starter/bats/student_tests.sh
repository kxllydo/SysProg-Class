#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Client connects to server" {
    run ./dsh -c -i 127.0.0.1 -p $PORT <<< "exit"
    [ "$status" -eq 0 ]
}


@test "Server starts and connects to port" {
    run nc -z localhost $PORT
    [ "$status" -eq 0 ]
}


@test "stop-server command stops server" {
    run ./dsh -c -i 127.0.0.1 -p $PORT <<EOF
stop-server
EOF
    [ "$status" -eq 0 ]
    
    # Verify server stopped
    run nc -z localhost $PORT
    [ "$status" -ne 0 ]
}

@test "Execute simple command via client" {
    run ./dsh -c -i 127.0.0.1 -p $PORT <<EOF
echo test
exit
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "test" ]]
}

@test "Invalid command returns error" {
    run ./dsh -c -i 127.0.0.1 -p $PORT <<EOF
invalidcommand
exit
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "error" ]]
}

@test "cd command changes directory" {
    tmpdir=$(mktemp -d)
    run ./dsh -c -i 127.0.0.1 -p $PORT <<EOF
cd $tmpdir
pwd
exit
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "$tmpdir" ]]
    rmdir $tmpdir
}

@test "Multiple sequential clients" {
    run ./dsh -c -i 127.0.0.1 -p $PORT <<EOF
echo first
exit
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "first" ]]

    run ./dsh -c -i 127.0.0.1 -p $PORT <<EOF
echo second
exit
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "second" ]]
}
