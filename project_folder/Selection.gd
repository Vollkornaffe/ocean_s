extends ReferenceRect


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

var stored_position = Vector2(0,0)

signal update_selection(position, size)

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if Input.is_action_just_released("ui_mouse_left"):
		emit_signal("update_selection", get_position(), get_size())
	var mouse_position = get_viewport().get_mouse_position()
	if Input.is_action_pressed("ui_mouse_left"):
		var diff = mouse_position - stored_position
		var pos = stored_position
		if diff.x < 0.0:
			pos.x += diff.x
			diff.x *= -1.0
		if diff.y < 0.0:
			pos.y += diff.y
			diff.y *= -1.0
		set_size(diff)
		set_position(pos)
	else:
		stored_position = mouse_position
		set_position(mouse_position)
		set_size(Vector2(0,0))
