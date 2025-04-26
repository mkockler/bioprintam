# pressure.py
from kivy.app import App
from kivy.uix.screenmanager import Screen
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.button import Button
from kivy.uix.label import Label

class PressureScreen(Screen):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        layout = BoxLayout(orientation='vertical', spacing=10, padding=10)
        layout.add_widget(Label(text="Set Pressure (psi)"))

        pressures = [str(v) for v in range(5, 55, 5)]  # 5, 10, 15, ..., 50

        for p in pressures:
            btn = Button(text=f"{p} psi")
            btn.bind(on_press=self.select_pressure)
            layout.add_widget(btn)

        back_btn = Button(text="Back")
        back_btn.bind(on_press=self.go_back)
        layout.add_widget(back_btn)

        self.add_widget(layout)

    def select_pressure(self, instance):
        App.get_running_app().pressure = instance.text  # Save selected pressure globally
        self.manager.current = 'home'  # Return to home screen

    def go_back(self, instance):
        self.manager.current = 'home'
