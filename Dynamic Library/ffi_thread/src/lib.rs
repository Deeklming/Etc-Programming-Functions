#[no_mangle]
pub extern fn process(){
    println!("process start\n");
    let handles: Vec<_> = (0..8).map(|_| {
        std::thread::spawn(|| {
            let mut x: i64 = 0;
            let mut y: i64 = 0;
            let mut z: i64 = 0;
            let mut a: i64 = 0;
            let mut b: i64 = 0;
            for _ in 0..200_000_000{
                x += 1;
                y += 1;
                z += 1;
                a += 1;
                b += 1;
            }
            println!("x:{}, y:{}, z:{}, a:{}, b:{}", x, y, z, a, b);
            x
        })
    }).collect();
    for h in handles{
        println!("Thread finished with count = {}*5",
        h.join().map_err(|_| "Colud not join a thread!").unwrap());
    }
    println!("\nprocess end");
}