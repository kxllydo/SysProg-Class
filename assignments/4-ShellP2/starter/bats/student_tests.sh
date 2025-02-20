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

