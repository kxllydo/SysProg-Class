#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Empty in first pipe" {
    run "./dsh" <<EOF                
echo "" | grep "anything"
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="dsh3>dsh3>cmdloopreturned1"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Multiple pipes" {
    run "./dsh" <<EOF                
echo "multiple pipes" | grep -o "pipes" | wc -c
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="6dsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Pipe with unknown command" {
    run "./dsh" <<EOF                
echo "hi" | bruh
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="execvp:Nosuchfileordirectorydsh3>dsh3>dsh3>cmdloopreturned250"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Pipe with special characters" {
    run "./dsh" <<EOF                
echo "hi@" | grep "@"
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="hi@dsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Pipe with quoted strings" {
    run "./dsh" <<EOF                
echo "hello 'world'" | grep "world"
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="hello'world'dsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Excess pipes" {
    run ./dsh <<EOF
ls | grep .c | wc -l | cat | more | cat | cat | cat | cat | cat
EOF
 # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="dsh3>error:pipinglimitedto8commandsdsh3>cmdloopreturned-2"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "List files" {
    current=$(pwd)
    run "${current}/dsh" <<EOF
ls
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="batsdragon.cdshdsh_cli.cdshlib.cdshlib.hmakefiledsh3>dsh3>cmdloopreturned0"
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
    expected_output="...bats.debugdragon.cdshdsh_cli.cdshlib.cdshlib.hmakefiledsh3>dsh3>cmdloopreturned0"
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
    expected_output=$(printf "hi\ndsh3> dsh3> \ncmd loop returned 0")
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
    expected_output=$(printf "cd: No such file or directory\ndsh3> dsh3> \ncmd loop returned 22")
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
    expected_output+=$'\n'"dsh3> dsh3> "
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
    expected_output+=$'\n'"dsh3> dsh3> "
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
    expected_output+=$'\n'"dsh3> dsh3> "
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
rc
EOF
    expected_output=$(printf "2\ndsh2> dsh2>\ncmd loop returned 0")
    echo "${stripped_output} -> ${expected_output}"

    [ "$status" -eq 0 ]
    [ "${lines[1]}"="$expected_output"  ]
}

@test "EXTRA CREDIT - Return code works" {
    current=$(pwd)
    run "${current}/dsh" <<EOF
snowman
rc
EOF
    expected_output=$(printf "2\ndsh2> dsh2>\ncmd loop returned 0")
    echo "${stripped_output} -> ${expected_output}"

    [ "$status" -eq 0 ]
    [ "${lines[1]}"="$expected_output"  ]
}