extends Node2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

var t = 0

# Called when the node enters the scene tree for the first time.
func _ready():
	pass

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta): 
#	pass

func _process(delta):
	t += delta
	var pp = get_node("PassiveParticles2D")
	if t > 2.0:
		var amount = pp.get_amount() * 2 
		pp.set_amount(amount)
		print(amount)
		print(Engine.get_frames_per_second())
		t = 0
	get_node("GDBoids").write_to_particles(pp)

func _physics_process(delta):
	get_node("GDBoids").physics_process(delta)
