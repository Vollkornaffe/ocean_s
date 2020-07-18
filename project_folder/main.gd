extends Node2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

var t = 0

# Called when the node enters the scene tree for the first time.
func _ready():
	get_node("SDF").calculate_from_polygon(1.0, 1000, 1000, get_node("Polygon2D"))
	pass

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta): 
#	pass

func _process(delta):
	t += delta
	var pp = get_node("PassiveParticles2D")
	var amount = pp.get_amount()
	if t > 2.0 and amount < 16384:
		pp.set_amount(amount * 2)
		t = 0
	get_node("GDBoids").write_to_particles(pp)

func _physics_process(delta):
	get_node("GDBoids").physics_process(get_viewport().get_mouse_position(), delta)
