# XML bounding-box comparison for Video Precessing
C++ Class for reading XML files and compare the bounding boxes stored in them using overlapping based measures.

## Configuration
Add the video.xml and video.xsd files to the Resource Files of the visual studio project.

XML files to read must have the following schema: 

	<video name="video1" algorithm="1">
		<frame no="1">
			<roi no="1" x="123" y="245" w="56" h="12" weight="34.234" />
			<roi no="2" x="34" y="33" w="323" h="33" weight="12.88" />
			<roi no="3" x="12" y="22" w="12" h="333" weight="1.2342" />	
		</frame>
		<frame no="2">
			<roi no="1" x="123" y="245" w="56" h="12" weight="34.234" />
			<roi no="2" x="34" y="33" w="323" h="33" weight="12.88" />
			<roi no="3" x="12" y="22" w="12" h="333" weight="1.2342" />	
		</frame>
		<frame no="3">
			<roi no="1" x="123" y="245" w="56" h="12" weight="34.234" />
			<roi no="2" x="34" y="33" w="323" h="33" weight="12.88" />
			<roi no="3" x="12" y="22" w="12" h="333" weight="1.2342" />	
		</frame>
	</video>

