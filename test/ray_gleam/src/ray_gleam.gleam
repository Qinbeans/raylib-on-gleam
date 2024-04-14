@external(erlang, "ray_erl", "raylib_close")
pub fn raylib_close() -> Int

@external(erlang, "ray_erl", "raylib_init")
pub fn raylib_init(width: Int, height: Int, title: String) -> Int

@external(erlang, "ray_erl", "raylib_should_close")
pub fn raylib_should_close() -> Bool

pub fn event_loop() -> Int {
  case raylib_should_close() {
    True -> event_loop()
    _ -> 0
  }
}

pub fn main() {
  raylib_init(800, 450, "Hello Gleam")
  event_loop()
  raylib_close()
}
