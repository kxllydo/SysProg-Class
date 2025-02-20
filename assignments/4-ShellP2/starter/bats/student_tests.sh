#!/usr/bin/env bats

@test "List files" {
    current=$(pwd)
    run "${current}/dsh" <<EOF
ls
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="$(ls | tr -d '[:space:]')dsh2>dsh2>cmdloopreturned0"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "List files with flag" {
    current=$(pwd)
    run "${current}/dsh" <<EOF
ls -a
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="$(ls -a| tr -d '[:space:]')dsh2>dsh2>cmdloopreturned0"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "List files with invalid argument" {
    current=$(pwd)
    run "${current}/dsh" <<EOF
ls a
EOF
    stripped_output=$(echo "$output")
    expected_output=$(printf "ls: cannot access 'a': No such file or directory\ndsh2> dsh2> \ncmd loop returned 0")
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Echo command" {
    current=$(pwd)
    run "${current}/dsh" <<EOF
echo "hi"
EOF
    stripped_output=$(echo "$output")
    expected_output=$(printf "hi\ndsh2> dsh2> \ncmd loop returned 0")
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Change to nonexisting directory" {
    current=$(pwd)
    run "${current}/dsh" <<EOF
cd bruh
EOF
    stripped_output=$(echo "$output")
    expected_output=$(printf "cd: No such file or directory\ndsh2> dsh2> \ncmd loop returned 0")
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}


@test "Print working directory" {
    current=$(pwd)
    run "${current}/dsh" <<EOF
pwd
EOF
    stripped_output=$(echo "$output")
    expected_output="$(pwd)"
    expected_output+=$'\n'"dsh2> dsh2> "
    expected_output+=$'\n'"cmd loop returned 0"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Exit successfully" {
    current=$(pwd)
    run "${current}/dsh" <<EOF
exit
EOF
    [ "$status" -eq 0 ]
}

@test "Uname" {
    current=$(pwd)
    run "${current}/dsh" <<EOF
uname
EOF
    stripped_output=$(echo "$output")
    expected_output="$(uname)"
    expected_output+=$'\n'"dsh2> dsh2> "
    expected_output+=$'\n'"cmd loop returned 0"
    echo "${stripped_output} -> ${expected_output}"


    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Uname with flag" {
    current=$(pwd)
    run "${current}/dsh" <<EOF
uname -a
EOF
    stripped_output=$(echo "$output")
    expected_output="$(uname -a)"
    expected_output+=$'\n'"dsh2> dsh2> "
    expected_output+=$'\n'"cmd loop returned 0"
    echo "${stripped_output} -> ${expected_output}"


    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}


@test "Print dragon" {
    current=$(pwd)
    run "${current}/dsh" <<EOF
dragon
EOF
    stripped_output=$(echo "$output")

    [ "$status" -eq 0 ]
}

@test "No command" {
    current=$(pwd)
    run "${current}/dsh" <<EOF

EOF
    [ "$status" -eq 0 ]
}

@test "Invalid command" {
    current=$(pwd)
    run "${current}/dsh" <<EOF
snowman
EOF
    [ "$status" -eq 0 ]
}

# @test "Change directory" {
#     current=$(pwd)

#     cd /tmp
#     mkdir -p dsh-test

#     run "${current}/dsh" <<EOF                
# cd dsh-test
# pwd
# EOF

#     # Strip all whitespace (spaces, tabs, newlines) from the output
#     stripped_output=$(echo "$output" | tr -d '[:space:]')

#     # Expected output with all whitespace removed for easier matching
#     expected_output="/tmp/dsh-testdsh2>dsh2>dsh2>cmdloopreturned0"

#     # These echo commands will help with debugging and will only print
#     #if the test fails
#     echo "Captured stdout:" 
#     echo "Output: $output"
#     echo "Exit Status: $status"
#     echo "${stripped_output} -> ${expected_output}"

#     # Check exact match
#     [ "$stripped_output" = "$expected_output" ]

#     # Assertions
#     [ "$status" -eq 0 ]
# }

# @test "Check if shell starts successfully" {
#     run $TEST_SHELL <<< "exit"
#     [ "$status" -eq 0 ]
# }

# @test "Test 'cd' command to a valid directory" {
#     run $TEST_SHELL <<< "cd /tmp\npwd\nexit"
#     [ "$status" -eq 0 ]
#     [[ "$output" == *"/tmp"* ]]
# }

# @test "Test 'cd' command to an invalid directory" {
#     run $TEST_SHELL <<< "cd /nonexistent\nexit"
#     [ "$status" -eq 0 ] # Should not crash
#     [[ "$output" == *"cd: "* ]]
# }

# @test "Test 'ls' command" {
#     run $TEST_SHELL <<< "ls\nexit"
#     [ "$status" -eq 0 ]
#     [[ "$output" != "" ]]
# }

# @test "Test 'echo' command" {
#     run $TEST_SHELL <<< "echo Hello, world!\nexit"
#     [ "$status" -eq 0 ]
#     [[ "$output" == *"Hello, world!"* ]]
# }

# @test "Test handling of empty input" {
#     run $TEST_SHELL <<< "\nexit"
#     [ "$status" -eq 0 ]
# }

# @test "Test execution of invalid command" {
#     run $TEST_SHELL <<< "invalidcommand\nexit"
#     [ "$status" -eq 0 ] # Should not crash
#     [[ "$output" == *"execvp: "* ]]
# }

# @test "Test exit command" {
#     run $TEST_SHELL <<< "exit"
#     [ "$status" -eq 0 ]
# }