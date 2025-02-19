#!/usr/bin/env bats

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF
ls
EOF
    [ "$status" -eq 0 ]
}

@test "Check if shell starts successfully" {
    run $TEST_SHELL <<< "exit"
    [ "$status" -eq 0 ]
}

@test "Test 'cd' command without arguments (should change to home directory)" {
    initial_dir=$(pwd)
    home_dir=$(getenv HOME)
    run $TEST_SHELL <<< "cd\npwd\nexit"
    final_dir=$(echo "$output" | tail -n 2 | head -n 1)
    [ "$final_dir" = "$home_dir" ]
}

@test "Test 'cd' command to a valid directory" {
    run $TEST_SHELL <<< "cd /tmp\npwd\nexit"
    [ "$status" -eq 0 ]
    [[ "$output" == *"/tmp"* ]]
}

@test "Test 'cd' command to an invalid directory" {
    run $TEST_SHELL <<< "cd /nonexistent\nexit"
    [ "$status" -eq 0 ] # Should not crash
    [[ "$output" == *"cd: "* ]]
}

@test "Test 'ls' command" {
    run $TEST_SHELL <<< "ls\nexit"
    [ "$status" -eq 0 ]
    [[ "$output" != "" ]]
}

@test "Test 'echo' command" {
    run $TEST_SHELL <<< "echo Hello, world!\nexit"
    [ "$status" -eq 0 ]
    [[ "$output" == *"Hello, world!"* ]]
}

@test "Test handling of empty input" {
    run $TEST_SHELL <<< "\nexit"
    [ "$status" -eq 0 ]
}

@test "Test execution of invalid command" {
    run $TEST_SHELL <<< "invalidcommand\nexit"
    [ "$status" -eq 0 ] # Should not crash
    [[ "$output" == *"execvp: "* ]]
}

@test "Test exit command" {
    run $TEST_SHELL <<< "exit"
    [ "$status" -eq 0 ]
}