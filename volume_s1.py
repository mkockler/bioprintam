#volume_s1.py
from kivy.uix.screenmanager import Screen
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.label import Label
from kivy.uix.button import Button

class VolumeS1Screen(Screen):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        layout = BoxLayout(orientation='vertical', spacing=10, padding=10)

        layout.add_widget(Label(text="Volume S1 (uL)"))
        for value in ["10", "20", "30", "40", "50"]:
            layout.add_widget(Button(text=f"{value} uL"))

        layout.add_widget(Button(text="Back", on_press=self.go_back))
        self.add_widget(layout)

    def go_back(self, instance):
        self.manager.current = 'home'
