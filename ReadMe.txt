F�r att ladda in Direct3D
- L�gg in "Microsoft DirectX SDK (June 2010)" i l�mplig mapp.
- �ppna visual studio och h�ger klicka p� "Direct3DWrapper, �ppna properties, v�lj Configuration: all configuration.
	- VC++ Directories
		- Under "Include Directories", $(DXSDK_DIR) + "\Include"
		- Under "Library Directories", $(DXSDK_DIR) + "\Lib\"
		Om det inte funkar kolla om DXSDK_DIR finns under milj�variabler. Om det fortfarande inte funkar. L�s det som nedan. 
		(- Under "Include Directories", l�gg till l�nken till d�r SDK placerades + "\Include"
		- Under "Library Directories", l�gg till l�nken till d�r SDK placerades + "\Lib\x86")
		(Obs inbland tycker visual studio om att �ndra "(" till %28 och ")" till %29, d� f�r man �ndra till paranteser manuellt. 


L�gg till samma l�nkningar f�r "Game". 



F�r att l�gga till wrapper till ett nytt project:
- L�gg till Direct3DWrapper i samma solution som det andra projektet, l�gg till ..\Direct3DWrapper\ under 
	C/C++ >> General >> additional include directories. 

Samt g� in under C/C++ General och l�gg till "..\Direct3DWrapper" under "additional include Directories"

- L�gg till d3dx9.lib och d3d9.lib under Linker >> Input >> additional dependencies (Under configururation: debug samt under release)
- �ndra wrappern till ett statiskt bibliotek under Configuration Properties >> General >> Configuration Type, under General 
	�ndra �ven Character set till "use multi-byte character set"

Man b�r dessutom h�gerklicka p� solution och v�lja properties samt project dependencies.

Dessutom m�ste man l�gga till Direct3DWrapper under reference f�r "Game". Reference hittar man i direkt under "Game" under solution explorer. 