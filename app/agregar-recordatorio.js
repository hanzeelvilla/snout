import { Stack } from "expo-router";
import AgregarRecordatorioPage from "../components/AgregarRecordatorioPage";
import { MenuIcon } from "../components/Icons";

export default function AgregarRecordatorio() {
  return (
    <>
      <Stack.Screen
        name="agregar-mascota"
        options={{ headerRight: () => <MenuIcon /> }}
      />
      <AgregarRecordatorioPage />
    </>
  );
}
