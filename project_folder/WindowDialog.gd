extends WindowDialog

# Declare member variables here. Examples:
# var a = 2
# var b = "text"

var init = false
const SpeciesDetails = preload("res://SpeciesDetails.tscn")

# Called when the node enters the scene tree for the first time.
func _ready():
	var species_list = get_node("MarginContainer/ScrollContainer/VBoxContainer")
	species_list.add_child(SpeciesDetails.instance())
	species_list.add_child(SpeciesDetails.instance())
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if Input.is_action_just_pressed("ui_accept"):
		if !init:
			popup_centered_ratio()
			init = true
		popup(get_rect())
	pass
