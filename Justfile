cpp:
    cd cpp && xmake
rust:
    cd rust && cargo build

fmt:
    cd cpp && xmake format
    cd rust && cargo fmt --all

run-rs:
    cd rust && cargo run

run-cpp:
    cd cpp && xmake run
