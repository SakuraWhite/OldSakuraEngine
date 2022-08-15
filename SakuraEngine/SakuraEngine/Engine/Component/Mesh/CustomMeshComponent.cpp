#include "CustomMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

CCustomMeshComponent::CCustomMeshComponent()
{

}

void CCustomMeshComponent::CreateMesh(FMeshRenderingData& MeshData, string& InPath)
{
	//判断是否为OBJ格式

		//拿到文件大小
	unsigned int FileSize = get_file_size_by_filename(InPath.c_str()); //判断导入的文件有多大
	//根据文件大小创建BUFF
	char* Buff = new char[FileSize + 1];//创建BUFF 记得+1 不然会崩溃
	//初始化buff
	memset(Buff, 0, FileSize + 1); //记得+1 不然会崩溃

	//提取BUFF
	get_file_buf(InPath.c_str(), Buff);//拿到文件


	if (!LoadObjFromBuff(Buff, FileSize, MeshData))
	{

	}

	delete Buff;//销毁buff
}

bool CCustomMeshComponent::LoadObjFromBuff(char* InBuff, uint32_t InBuffSize, FMeshRenderingData& MeshData)
{

	if (InBuffSize > 0)//文件的大小一定要大于零否则没有意义
	{
		stringstream BuffStream(InBuff); //遍历Buff文件的内容
		char TmpLine[256] = { 0 };//提取的临时变量
		string MidTmp; //作为中间值

		for (; !BuffStream.eof();)  //   "!BuffStream.eof()"代表是否结束了遍历
		{
			memset(TmpLine, 0, 256);//下次遍历需要清除

			//判断读取的每一行数据
			BuffStream.getline(TmpLine, 256);//一行一行的读
			if (strlen(TmpLine) > 0)//读出来的数据大于零才有意义
			{
				if (TmpLine[0] == 'v')//读取的每行的数据开头 是否是V开头 还是VT F等等
				{
					stringstream LineStream(TmpLine);//再次遍历
					LineStream >> MidTmp;//作为中间值
					if (TmpLine[1] == 'n') //判断第二个字符是否为n  n就是法线
					{

					}
					else if (TmpLine[1] == 't')//如果不为法线则判断是否为t  t就是贴图UV
					{

					}
					else  //如果没有第二个字符 那么说明是顶点
					{
						//然后把顶点保存然后添加  先添加一个
						MeshData.VertexData.push_back(FVertex(
							XMFLOAT3(), XMFLOAT4(Colors::White)));

						//拿到添加后的位置
						int TopIndex = MeshData.VertexData.size() - 1;//拿到索引
						XMFLOAT3& Float3InPos = MeshData.VertexData[TopIndex].Position;//之后确定点的位置

						//解析位置
						LineStream >> Float3InPos.x;//转制成对应的内容
						LineStream >> Float3InPos.y;//转制成对应的内容
						LineStream >> Float3InPos.z;//转制成对应的内容
					}

				}

				else if (TmpLine[0] == 'f')  //如果不是v开头 那么则有可能是绘制指令 判断是否是绘制指令“f” 
				{
					stringstream LineStream(TmpLine);//再次遍历 解析到流里
					LineStream >> MidTmp;//提取首字母“f” 相当于标签

					char SaveLineString[256] = { 0 };
					char TmpBuff[256] = { 0 };

					for (size_t i = 0; i < 3; i++)
					{
						memset(SaveLineString, 0, 256);

						//输入一行数据
						LineStream >> SaveLineString;//将线性的流输入到准备好的容器里
						//找到索引位置
						int StringPosA = find_string(SaveLineString, "/", 0);//寻找字符 “/”
						memset(TmpBuff, 0, 256);//将TmpBuff清零
						//将数据提取
						char* VPosIndex = string_mid(SaveLineString, TmpBuff, 0, StringPosA);
						//放入索引容器里
						MeshData.IndexData.push_back(atoi(VPosIndex) - 1);

						//找到第二个索引位置 也就是纹理数据index
						int StringPosB = find_string(SaveLineString, "/", StringPosA + 1);//寻找字符 “/”
						memset(TmpBuff, 0, 256);//将TmpBuff清零
						//提取纹理Index数据
						char* TexcoordIndex = string_mid(SaveLineString, TmpBuff, StringPosA + 1, StringPosB - (StringPosA + 1));

						//法线Index提取
						memset(TmpBuff, 0, 256);//将TmpBuff清零
						//提取纹理Index数据
						char* NormalIndex = string_mid(SaveLineString, TmpBuff, StringPosB + 1, strlen(SaveLineString) - (StringPosB + 1));

					}

				}

			}

		}

		return true;
	}


	return false;
}


//构建哈希key
void CCustomMeshComponent::BuildKey(size_t& OutHashKey, std::string& InPath)
{
	std::hash<string> FloatHash;

	OutHashKey = 3;
	OutHashKey += FloatHash(InPath);
}