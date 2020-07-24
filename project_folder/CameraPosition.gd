extends Position2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

var speed = 500

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	var new_pos = get_position()
	if Input.is_action_pressed("ui_up"):
		new_pos.y -= delta * speed
	if Input.is_action_pressed("ui_down"):
		new_pos.y += delta * speed
	if Input.is_action_pressed("ui_left"):
		new_pos.x -= delta * speed
	if Input.is_action_pressed("ui_right"):
		new_pos.x += delta * speed
	set_position(Vector2(clamp(new_pos.x, 100, 7900), clamp(new_pos.y, 100, 1900)))
	pass

