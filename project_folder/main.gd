extends Node2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

var t = 0

# Called when the node enters the scene tree for the first time.
func _ready():
	get_node("Boids").set_passive_particles(get_node("PassiveParticles2D"))
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass

func _process(delta):
	t += delta
	if t > 2.0:
		var pp = get_node("PassiveParticles2D")
		var amount = pp.get_amount() * 2 
		pp.set_amount(amount)
		print(amount)
		print(Engine.get_frames_per_second())
		t = 0
