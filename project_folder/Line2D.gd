extends Line2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	var p = get_node("../Sprite").get_position()
	set_point_position(0, p)
	set_point_position(1, p + get_node("../SDF").get_gradient(p) * 100)
	print(get_node("../SDF").get_value(p))
	pass
